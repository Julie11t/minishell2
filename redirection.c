/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <jhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:44:11 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/25 21:44:11 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *cmd)
{
	int		i;
	
	i = 0;
	while (cmd->argv && cmd->argv[i])
		free(cmd->argv[i++]);
	free(cmd->argv);
	free(cmd);
}

void	execute_redirection(t_command *cmd, t_data *data)
{
	int		saved_in;
	int		saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (cmd->infile != STDIN_FILENO)
		dup2(cmd->infile, STDIN_FILENO);
	if (cmd->outfile != STDOUT_FILENO)
		dup2(cmd->outfile, STDOUT_FILENO);
	if (is_builtin(cmd->argv[0]))
		run_builtin(cmd->argv, data);
	// else
	// 	run_external(cmd->argv, data);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	if (cmd->infile != STDIN_FILENO)
		close(cmd->infile);
	if (cmd->outfile != STDOUT_FILENO)
		close(cmd->outfile);
}

void	init_loop(t_command **cmd, t_token **cur, char ***args, t_token *tokens)
{
	int		argc;
	t_token	*tmp;

	argc = 0;
	*cmd = malloc(sizeof(t_command));
	if (!*cmd)
		return;
	(*cmd)->infile = STDIN_FILENO;
	(*cmd)->outfile = STDOUT_FILENO;
	tmp = tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == COMMAND || tmp->type == WORD)
			argc++;
		tmp = tmp->next;
	}
	*args = malloc(sizeof(char *) * (argc + 1));
	if (!*args)
	{
		free(*cmd);
		*cmd = NULL;
		return;
	}
	*cur = tokens;
}

int	handle_token(t_token **cur, t_command *cmd, char **args, int *i)
{
	t_token *token;

	token = *cur;
	if (token->type == REDIR_OUT && token->next)
	{
		cmd->outfile = open(token->next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		*cur = token->next;
	}
	else if (token->type == REDIR_IN && token->next)
	{
		cmd->infile = open(token->next->value, O_RDONLY);
		*cur = token->next;
	}
	else if (token->type == APPEND && token->next)
	{
		cmd->outfile = open(token->next->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		*cur = token->next;
	}
	else if (token->type == COMMAND || token->type == WORD)
	{
		args[(*i)++] = ft_strdup(token->value);
	}
	if (cmd->outfile == -1 || cmd->infile == -1)
		return (-1);
	return (0);
}

t_command	*parse_tokens_to_command(t_token *tokens)
{
	t_command	*cmd;
	t_token		*cur;
	char		**args;
	int			i;

	init_loop(&cmd, &cur, &args, tokens);
	if (!cmd || !args)
		return (NULL);
	i = 0;
	while (cur && cur->type != PIPE)
	{
		if (handle_token(&cur, cmd, args, &i) == -1)
		{
			perror("minishell");
			free(args);
			free(cmd);
			return (NULL);
		}
		cur = cur->next;
	}
	args[i] = NULL;
	cmd->argv = args;
	return (cmd);
}

// t_command	*parse_tokens_to_command(t_token *tokens)
// {
// 	t_command	*cmd;
// 	t_token		*cur;
// 	char		**args;
// 	int			i;
// 	int			argc;

//     cmd = malloc(sizeof(t_command));
//     if (!cmd)
//     {
//         return (NULL);
//     }
// 	cmd->infile = STDIN_FILENO;
// 	cmd->outfile = STDOUT_FILENO;
// 	argc = 0;
// 	cur = tokens;
// 	while (cur && cur->type != PIPE)
// 	{
// 		if (cur->type == COMMAND || cur->type == WORD)
// 			argc++;
// 		cur = cur->next;
// 	}
// 	args = malloc(sizeof(char *) * (argc + 1));
// 	cur = tokens;
// 	i = 0;
// 	while (cur)
// 	{
// 		if (cur->type == REDIR_OUT && cur->next)
// 		{
// 			cmd->outfile = open(cur->next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 			cur = cur->next;
// 		}
// 		else if (cur->type == REDIR_IN && cur->next)
// 		{
// 			cmd->infile = open(cur->next->value, O_RDONLY);
// 			cur = cur->next;
// 		}
// 		else if (cur->type == APPEND && cur->next)
// 		{
// 			cmd->outfile = open(cur->next->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
// 			cur = cur->next;
// 		}
// 		else if (cur->type == COMMAND || cur->type == WORD)
// 		{
// 			args[i++] = ft_strdup(cur->value);
// 		}
//         if (cmd->outfile == -1)
//         {
// 	        perror("minishell");
// 	        free(args);
//             free(cmd);
//     	    return (NULL);
//         }
// 		cur = cur->next;
// 	}
// 	args[i] = NULL;
// 	cmd->argv = args;
// 	return (cmd);
// }

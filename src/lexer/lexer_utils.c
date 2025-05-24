/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:59:07 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_token(char *input, int *i, t_token **tokens, int advance)
{
	t_token	*token;
	char	*value;

	if (is_quote(input[*i]))
		value = extract_quoted_string(input, i);
	else if (is_operator(input[*i]))
	{
		token = create_token(get_operator_type(input + *i, &advance),
				ft_substr(input, *i, advance));
		*i += advance;
		add_token(tokens, token);
		return ;
	}
	else
		value = extract_word(input, i);
	token = create_token(TOKEN_WORD, value);
	add_token(tokens, token);
}

char	*extract_word(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && !is_whitespace(str[*i]) && !is_operator(str[*i])
		&& !is_quote(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}
